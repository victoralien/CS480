#!/usr/bin/env python3
"""
Memory Allocation Simulation Results Grapher
Generates comprehensive graphs for First Fit vs Best Fit comparison
Assignment 3 - Operating Systems
"""

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os
import sys

def read_simulation_results():
    """Read final simulation results from file"""
    results = {}
    try:
        with open('simulation_results.txt', 'r') as f:
            for line in f:
                if ':' in line:
                    key, value = line.strip().split(': ')
                    results[key] = float(value)
        return results
    except FileNotFoundError:
        print("Error: simulation_results.txt not found. Run the simulation first.")
        return None

def read_time_series_data():
    """Read time series fragmentation data from CSV file"""
    try:
        df = pd.read_csv('fragmentation_data.txt')
        return df
    except FileNotFoundError:
        print("Error: fragmentation_data.txt not found. Run the simulation first.")
        return None

def create_comparison_bar_chart(results):
    """Create comprehensive bar chart comparing all metrics"""
    metrics = ['External\nFragments', 'Nodes\nTraversed', 'Requests\nDenied (%)']
    first_fit_values = [
        results['FirstFit_Fragments'], 
        results['FirstFit_Nodes'], 
        results['FirstFit_Denied']
    ]
    best_fit_values = [
        results['BestFit_Fragments'], 
        results['BestFit_Nodes'], 
        results['BestFit_Denied']
    ]
    
    x = np.arange(len(metrics))
    width = 0.35
    
    fig, ax = plt.subplots(figsize=(12, 8))
    bars1 = ax.bar(x - width/2, first_fit_values, width, 
                   label='First Fit', color='#4CAF50', alpha=0.8, edgecolor='black')
    bars2 = ax.bar(x + width/2, best_fit_values, width, 
                   label='Best Fit', color='#FF9800', alpha=0.8, edgecolor='black')
    
    ax.set_xlabel('Performance Metrics', fontsize=12, fontweight='bold')
    ax.set_ylabel('Values', fontsize=12, fontweight='bold')
    ax.set_title('Memory Allocation Algorithm Performance Comparison\n' +
                '10,000 Requests (3-10 units each, 256KB total memory)', 
                fontsize=14, fontweight='bold')
    ax.set_xticks(x)
    ax.set_xticklabels(metrics, fontsize=11)
    ax.legend(fontsize=12)
    ax.grid(True, alpha=0.3, axis='y')
    
    # Add value labels on bars
    def add_value_labels(bars):
        for bar in bars:
            height = bar.get_height()
            ax.annotate(f'{height:.3f}',
                       xy=(bar.get_x() + bar.get_width() / 2, height),
                       xytext=(0, 3),
                       textcoords="offset points",
                       ha='center', va='bottom', fontweight='bold')
    
    add_value_labels(bars1)
    add_value_labels(bars2)
    
    plt.tight_layout()
    plt.savefig('memory_allocation_comparison.png', dpi=300, bbox_inches='tight')
    plt.show()
    print("✓ Created: memory_allocation_comparison.png")

def create_individual_metric_charts(results):
    """Create separate charts for each metric with detailed analysis"""
    fig, axes = plt.subplots(2, 2, figsize=(15, 12))
    fig.suptitle('Detailed Performance Analysis: First Fit vs Best Fit', 
                 fontsize=16, fontweight='bold')
    
    algorithms = ['First Fit', 'Best Fit']
    colors = ['#4CAF50', '#FF9800']
    
    # External Fragments
    fragments_data = [results['FirstFit_Fragments'], results['BestFit_Fragments']]
    bars1 = axes[0,0].bar(algorithms, fragments_data, color=colors, alpha=0.8, edgecolor='black')
    axes[0,0].set_title('Average External Fragments per Request', fontweight='bold')
    axes[0,0].set_ylabel('Number of Fragments')
    axes[0,0].grid(True, alpha=0.3, axis='y')
    for i, v in enumerate(fragments_data):
        axes[0,0].text(i, v + max(fragments_data)*0.02, f'{v:.4f}', 
                      ha='center', va='bottom', fontweight='bold')
    
    # Nodes Traversed
    nodes_data = [results['FirstFit_Nodes'], results['BestFit_Nodes']]
    bars2 = axes[0,1].bar(algorithms, nodes_data, color=colors, alpha=0.8, edgecolor='black')
    axes[0,1].set_title('Average Nodes Traversed per Allocation', fontweight='bold')
    axes[0,1].set_ylabel('Number of Nodes')
    axes[0,1].grid(True, alpha=0.3, axis='y')
    for i, v in enumerate(nodes_data):
        axes[0,1].text(i, v + max(nodes_data)*0.02, f'{v:.4f}', 
                      ha='center', va='bottom', fontweight='bold')
    
    # Denied Requests
    denied_data = [results['FirstFit_Denied'], results['BestFit_Denied']]
    bars3 = axes[1,0].bar(algorithms, denied_data, color=colors, alpha=0.8, edgecolor='black')
    axes[1,0].set_title('Percentage of Requests Denied', fontweight='bold')
    axes[1,0].set_ylabel('Percentage (%)')
    axes[1,0].grid(True, alpha=0.3, axis='y')
    for i, v in enumerate(denied_data):
        axes[1,0].text(i, v + max(denied_data + [0.1])*0.02, f'{v:.4f}%', 
                      ha='center', va='bottom', fontweight='bold')
    
    # Performance Trade-off Analysis
    efficiency = [1/results['FirstFit_Nodes'], 1/results['BestFit_Nodes']]
    quality = [1/(results['FirstFit_Fragments']+0.001), 1/(results['BestFit_Fragments']+0.001)]
    
    axes[1,1].scatter([efficiency[0]], [quality[0]], s=200, c='#4CAF50', 
                     label='First Fit', alpha=0.8, edgecolors='black')
    axes[1,1].scatter([efficiency[1]], [quality[1]], s=200, c='#FF9800', 
                     label='Best Fit', alpha=0.8, edgecolors='black')
    axes[1,1].set_xlabel('Allocation Efficiency (1/Nodes)')
    axes[1,1].set_ylabel('Memory Quality (1/Fragments)')
    axes[1,1].set_title('Efficiency vs Quality Trade-off', fontweight='bold')
    axes[1,1].legend()
    axes[1,1].grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('detailed_performance_analysis.png', dpi=300, bbox_inches='tight')
    plt.show()
    print("✓ Created: detailed_performance_analysis.png")

def create_time_series_graphs(df):
    """Create time series graphs showing evolution over simulation"""
    fig, axes = plt.subplots(2, 1, figsize=(14, 10))
    fig.suptitle('Performance Evolution Over 10,000 Requests', 
                 fontsize=16, fontweight='bold')
    
    # Fragmentation over time
    axes[0].plot(df['Request'], df['FirstFit_Fragments'], 
                'o-', color='#4CAF50', linewidth=2, markersize=4, 
                label='First Fit', alpha=0.8)
    axes[0].plot(df['Request'], df['BestFit_Fragments'], 
                's-', color='#FF9800', linewidth=2, markersize=4, 
                label='Best Fit', alpha=0.8)
    axes[0].set_xlabel('Request Number')
    axes[0].set_ylabel('Average External Fragments')
    axes[0].set_title('Memory Fragmentation Evolution', fontweight='bold')
    axes[0].legend()
    axes[0].grid(True, alpha=0.3)
    axes[0].set_xlim(0, 10000)
    
    # Nodes traversed over time
    axes[1].plot(df['Request'], df['FirstFit_AvgNodes'], 
                'o-', color='#4CAF50', linewidth=2, markersize=4, 
                label='First Fit', alpha=0.8)
    axes[1].plot(df['Request'], df['BestFit_AvgNodes'], 
                's-', color='#FF9800', linewidth=2, markersize=4, 
                label='Best Fit', alpha=0.8)
    axes[1].set_xlabel('Request Number')
    axes[1].set_ylabel('Average Nodes Traversed')
    axes[1].set_title('Allocation Overhead Evolution', fontweight='bold')
    axes[1].legend()
    axes[1].grid(True, alpha=0.3)
    axes[1].set_xlim(0, 10000)
    
    plt.tight_layout()
    plt.savefig('performance_evolution.png', dpi=300, bbox_inches='tight')
    plt.show()
    print("✓ Created: performance_evolution.png")

def create_summary_report(results, df):
    """Generate a comprehensive text summary"""
    print("\n" + "="*80)
    print("MEMORY ALLOCATION SIMULATION - COMPREHENSIVE RESULTS")
    print("="*80)
    print(f"Simulation Parameters:")
    print(f"  • Total Requests: 10,000")
    print(f"  • Memory Size: 256 KB (128 units × 2 KB each)")
    print(f"  • Request Size Range: 3-10 units")
    print(f"  • Allocation/Deallocation: 50/50 random split")
    print("\n" + "-"*80)
    
    print(f"{'METRIC':<35} {'FIRST FIT':<15} {'BEST FIT':<15} {'ADVANTAGE':<15}")
    print("-"*80)
    
    # Fragmentation comparison
    ff_frag = results['FirstFit_Fragments']
    bf_frag = results['BestFit_Fragments']
    frag_advantage = "Best Fit" if bf_frag < ff_frag else "First Fit"
    print(f"{'External Fragments':<35} {ff_frag:<15.6f} {bf_frag:<15.6f} {frag_advantage:<15}")
    
    # Nodes traversed comparison
    ff_nodes = results['FirstFit_Nodes']
    bf_nodes = results['BestFit_Nodes']
    nodes_advantage = "First Fit" if ff_nodes < bf_nodes else "Best Fit"
    print(f"{'Nodes Traversed':<35} {ff_nodes:<15.6f} {bf_nodes:<15.6f} {nodes_advantage:<15}")
    
    # Denied requests comparison
    ff_denied = results['FirstFit_Denied']
    bf_denied = results['BestFit_Denied']
    denied_advantage = "Best Fit" if bf_denied < ff_denied else "First Fit"
    print(f"{'Requests Denied (%)':<35} {ff_denied:<15.6f} {bf_denied:<15.6f} {denied_advantage:<15}")
    
    print("="*80)
    
    # Analysis
    print("\nKEY FINDINGS:")
    print(f"• Fragmentation: Best Fit produces {((ff_frag-bf_frag)/ff_frag*100):.1f}% less fragmentation")
    print(f"• Efficiency: First Fit is {((bf_nodes-ff_nodes)/bf_nodes*100):.1f}% more efficient in search time")
    print(f"• Success Rate: {'Best Fit' if bf_denied < ff_denied else 'First Fit'} has better allocation success rate")
    
    if df is not None and len(df) > 0:
        final_ff_frag = df['FirstFit_Fragments'].iloc[-1]
        final_bf_frag = df['BestFit_Fragments'].iloc[-1]
        print(f"• Final fragmentation stabilized at FF:{final_ff_frag:.3f}, BF:{final_bf_frag:.3f}")

def main():
    print("Memory Allocation Simulation Results Analyzer")
    print("=" * 50)
    
    # Check for required files
    if not os.path.exists('simulation_results.txt'):
        print("Error: simulation_results.txt not found.")
        print("Please run the simulation first with: make run")
        return
    
    # Read results
    print("Reading simulation results...")
    results = read_simulation_results()
    if results is None:
        return
    
    # Read time series data
    print("Reading time series data...")
    df = read_time_series_data()
    
    # Generate summary report
    create_summary_report(results, df)
    
    # Create visualizations
    print("\nGenerating visualizations...")
    create_comparison_bar_chart(results)
    create_individual_metric_charts(results)
    
    if df is not None:
        create_time_series_graphs(df)
    else:
        print("⚠ Time series data not available - skipping evolution graphs")
    
    print(f"\n{'='*50}")
    print("Graph generation complete!")
    print("Files created:")
    print("  • memory_allocation_comparison.png")
    print("  • detailed_performance_analysis.png")
    if df is not None:
        print("  • performance_evolution.png")
    print(f"{'='*50}")

if __name__ == "__main__":
    # Check if matplotlib is available
    try:
        import matplotlib.pyplot as plt
        import pandas as pd
        import numpy as np
    except ImportError as e:
        print(f"Error: Required Python packages not installed.")
        print(f"Please install with: pip3 install matplotlib pandas numpy")
        print(f"Missing package: {e}")
        sys.exit(1)
    
    main()